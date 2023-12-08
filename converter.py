import os, sys

INDENT = "    "

def converter(benchmark_name, gap, USE_PREFETCH):
    f_raw = open(os.path.join("raws", benchmark_name + "_raw.cpp"), 'r')
    f_tester = open(os.path.join("testers", benchmark_name + ".cpp"), 'w')

    # Prefetcher Headers
    f_tester.write("#define _QUEUE " + gap + "\n\n")
    if USE_PREFETCH:
        f_tester.write("#include <mmintrin.h>\n")
        f_tester.write("#include <xmmintrin.h>\n")

    nextline = f_raw.readline()

    # Assume that we are not dealing with nested loops
    # Instructions in the landing pad
    head = []
    # Instructions in the actual loop
    body = []
    # Tail instructions
    tail = []
    # All variables that need to be recorded for prefetch for array indices
    index_list = []

    # Is this the first loop MACRO we encountered?
    first_macro = True

    # States can be:
    # 0 - not inside a loop that involves prefetching
    # 1 - inside a loop, instruction unrelated to prefetching
    # 2 - inside a loop, instruction related to prefetching
    state = 0
    while nextline != "":
        next_segment = nextline.split(' ')
        while next_segment[0] == "":
            next_segment = next_segment[1:]
        if next_segment[0] == "[[":
            # User annotations are consisted of
            # [[ LOOP <end> ]] - equivalent to `for (int _IT = 0; _IT < end; _IT++)`
            # [[ ADDR <var1> <var2> ... ]] - instructions after that will be used to generate the prefetcher address,
            #                                entries of <var1>, etc. will be used as prefetcher indices
            # [[ PREFETCH <array> <index> ]] - instructions to prefetch array[index]`
            # [[ OTHER ]] - instructions after that are unrelated to prefetcher generation
            # [[ ENDLOOP ]] - terminates the loop
            match next_segment[1]:
                case "LOOP":
                    if state != 0:
                        raise ValueError("Nested loops not supported!")
                    end = next_segment[2]
                    # _NEXT_AVAIL - the next available entry in the queue to be overwitten
                    # _NEXT_ACCESS - the next enetry in the queue for memory access
                    if first_macro:
                        head.append(INDENT + "int _NEXT_AVAIL = _QUEUE - 2;\n")
                        head.append(INDENT + "int _NEXT_ACCESS = 0;\n")
                        first_macro = False
                    else:
                        head.append(INDENT + "_NEXT_AVAIL = 0;\n")
                        head.append("    " + "_NEXT_ACCESS = 0;\n")

                    head.append(INDENT + f"for (int _IT = 0; _IT < _QUEUE; _IT++) " + "{\n")
                    # Q: What should we do if end < gap??
                    body.append(INDENT + f"for (int _IT = 0; _IT < {end} - _QUEUE; _IT++) " + "{\n")
                    tail.append(INDENT + f"for (int _IT = 0; _IT < _QUEUE; _IT++) " + "{\n")
                    state = 1
                case "ADDR":
                    if state == 0:
                        raise ValueError("Memory MACROS needs to be within loops")
                    # Restore all variables to their state in the prefetcher
                    vars_list = []
                    next_segment = next_segment[2:]
                    while next_segment[0] != "]]" and next_segment[0] != "]]\n":
                        vars_list.append(next_segment[0])
                        next_segment = next_segment[1:]
                    for var in vars_list:
                        body.append(2 * INDENT + f"{var} = {var}_QUEUE[_NEXT_AVAIL];\n")
                        id_q = f"{var}_QUEUE"
                        if not var in index_list:
                            # Initialize a new queue for previous iteration of the index
                            head.insert(0, INDENT + f"int {id_q}[_QUEUE];\n")
                            index_list.append(var)
                    state = 2
                case "PREFETCH":
                    if state == 0:
                        raise ValueError("Memory MACROS needs to be within loops")
                    array = next_segment[2]
                    index = next_segment[3]
                    if USE_PREFETCH:
                        head.append(2 * INDENT + f"_mm_prefetch((const char *)&{array}[{index}], _MM_HINT_T0);\n")
                        body.append(2 * INDENT + f"_mm_prefetch((const char *)&{array}[{index}], _MM_HINT_T0);\n")
                    
                case "OTHER":
                    if state == 0:
                        raise ValueError("Memory MACROS needs to be within loops")
                    if state == 2:
                        body.append(2 * INDENT + "_NEXT_AVAIL = (_NEXT_AVAIL + 1) % _QUEUE;\n")
                        for var in index_list:
                            id_q = f"{var}_QUEUE"
                            # Store the prefetched address into queue
                            head.append(2 * INDENT + f"{id_q}[_IT] = {var};\n")
                            body.append(2 * INDENT + f"{id_q}[_NEXT_AVAIL] = {var};\n")
                            # Load the address used for access out xof the queue
                            body.append(2 * INDENT + f"{var} = {id_q}[_NEXT_ACCESS];\n")
                            tail.append(2 * INDENT + f"{var} = {id_q}[_NEXT_ACCESS];\n")
                        body.append(2 * INDENT + "_NEXT_ACCESS = (_NEXT_ACCESS + 1) % _QUEUE;\n")
                        tail.append(2 * INDENT + "_NEXT_ACCESS = (_NEXT_ACCESS + 1) % _QUEUE;\n")
                    state = 1
                case "ENDLOOP":
                    if state == 0:
                        raise ValueError("Cannot terminate a loop that does not exist")
                    if state == 2:
                        for var in index_list:
                            id_q = f"{var}_QUEUE"
                            head.append(2 * INDENT + f"{id_q}[_IT] = {var};\n")
                            body.append(2 * INDENT + f"{id_q}[_NEXT_AVAIL] = {var};\n")
                            # Load the address used for access out xof the queue
                            body.append(2 * INDENT + f"{var} = {id_q}[_NEXT_ACCESS];\n")
                            tail.append(2 * INDENT + f"{var} = {id_q}[_NEXT_ACCESS];\n")
                        body.append(2 * INDENT + "_NEXT_ACCESS = (_NEXT_ACCESS + 1) % _QUEUE;\n")
                        tail.append(2 * INDENT + "_NEXT_ACCESS = (_NEXT_ACCESS + 1) % _QUEUE;\n")

                    head.append(INDENT + "}\n")
                    body.append(INDENT + "}\n")
                    tail.append(INDENT + "}\n")
                    f_tester.writelines(head)
                    f_tester.writelines(body)
                    f_tester.writelines(tail)
                    head = []
                    body = []
                    tail = []
                    index_list = []
                    state = 0
                case _:
                    raise ValueError("Unexpected user annotation: " + next_segment[1])
        else:
            match state:
                case 0:
                    f_tester.write(nextline)
                case 1:
                    if nextline != "\n":
                        body.append(INDENT + nextline)
                        tail.append(INDENT + nextline)
                case 2:
                    if nextline != "\n":
                        head.append(INDENT + nextline)
                        body.append(INDENT + nextline)
                case _:
                    raise ValueError("Unknown state: " + str(state))
        nextline = f_raw.readline()
    
    f_raw.close()
    f_tester.close()

if __name__ == "__main__":
    benchmark_name = sys.argv[1]
    gap = sys.argv[2]
    
    converter(benchmark_name, gap)