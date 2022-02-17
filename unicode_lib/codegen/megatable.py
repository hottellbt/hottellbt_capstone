#/usr/bin/python3

from ucd import eprint

# size of a (void*) in bytes
SIZEOF_POINTER = 8


def print_array(
        array_type,
        array_name,
        values,
        array_len = ""):

    def print_element(value):
        printed = str(value)
        print(printed, end=', ')

    print(f"{array_type} {array_name}[{array_len}] {{", end='')

    for val in values:
        print_element(val)

    print("};")


class MegaTable():

    def __init__(
            self,
            default_value,
            data_type,
            namespace_name,
            namespace_preamble = None,
            global_postamble = None,
            out_of_bounds_value = "0",
            value_print_converter = None,
            sizeof_chunk_elem = 1,
            index_type = "Unicode::codepoint_t"):

        self.default_value = default_value
        self.data_type = data_type
        self.namespace_name = namespace_name
        self.namespace_preamble = namespace_preamble
        self.global_postamble = global_postamble
        self.out_of_bounds_value = out_of_bounds_value
        self.value_print_converter = value_print_converter
        self.sizeof_chunk_elem = sizeof_chunk_elem
        self.index_type = index_type

        self.data = list()
        self.min_index = None
        self.max_index = None

    def ensure_capacity(self, _len):
        while len(self.data) < _len:
            self.data.append(self.default_value)

    def __getitem__(self, index):
        if len(self.data) <= index:
            return self.default_value
        return self.data[index]

    def __setitem__(self, index, value):
        if self.min_index is None or index < self.min_index:
            self.min_index = index
        if self.max_index is None or index > self.max_index:
            self.max_index = index

        self.ensure_capacity(index + 1)
        self.data[index] = value

    def chunkify(self, chunk_len):
        chunk = []

        if self.min_index is None:
            return

        for i in range(self.min_index, self.max_index+1):
            chunk.append(self[i])

            if len(chunk) >= chunk_len:
                yield chunk
                chunk = []

        if not len(chunk) == 0:
            yield chunk

    def dump(
            self, chunk_len,
            quiet = False):

        if not quiet:
            print(f"namespace {self.namespace_name} {{")

            if self.namespace_preamble is not None:
                print(self.namespace_preamble)

            print(f"constexpr size_t chunk_len = {chunk_len};")

        ram_size = 0
        chunk_no = 1

        chunk_pointer_cache = [] # tuple, chunk list and then name
        chunk_pointer_table = []


        def get_chunk_pointer(chunk):
            nonlocal chunk_no, ram_size

            def chunk_eq(c1, c2):
                min_size = min(len(c1), len(c2))
                for i in range(min_size):
                    if c1[i] != c2[i]:
                        return False
                return True


            for cached_chunk, cached_chunk_name in chunk_pointer_cache:
                if chunk_eq(cached_chunk, chunk):
                    return cached_chunk_name

            chunk_name = f"c{chunk_no}"

            chunk_pointer_cache.append((chunk, chunk_name))

            if not quiet:
                if self.value_print_converter is not None:
                    chunk_printed = [self.value_print_converter(x) for x in chunk]
                else:
                    chunk_printed = chunk
                print_array(self.data_type, chunk_name, chunk_printed)

            ram_size = ram_size + (len(chunk) * self.sizeof_chunk_elem)

            chunk_no = chunk_no + 1

            return chunk_name


        for chunk in self.chunkify(chunk_len):
            chunk_ptr = get_chunk_pointer(chunk)
            chunk_pointer_table.append(chunk_ptr)

        ram_size = ram_size + (len(chunk_pointer_table) * SIZEOF_POINTER)

        if not quiet:
            print_array(self.data_type + "*", "chunks", chunk_pointer_table)

            print(f"{self.data_type} lookup({self.index_type} i) {{")

            if self.min_index is None:
                print(f"return {out_of_bounds_value};")
            else:
                print(f"if (i > {self.max_index}) return {self.out_of_bounds_value};")
                if self.min_index > 0:
                    print(f"if (i < {self.min_index}) return {self.out_of_bounds_value};")
                    print(f"i -= {self.min_index};")
                print(f"auto chunk_index = i / chunk_len;")
                print(f"auto chunk_offset = i % chunk_len;")
                print(f"return chunks[chunk_index][chunk_offset];")

            print("}") # end of enclosing function
            print("}") # end of enclosing namespace

            if self.global_postamble is not None:
                print(self.global_postamble)

        return ram_size

    def dump_optimally(self):
        candidates = [32, 64, 128, 256, 512, 1028, 2048]

        best_size = None
        best_chunk_len = None

        for chunk_len in candidates:
            size = self.dump(chunk_len = chunk_len, quiet = True)
            eprint(f"chunk_len={chunk_len}, size={size}")

            if best_size is None or size < best_size:
                best_chunk_len = chunk_len
                best_size = size

        eprint(f"choice={best_chunk_len}")
        return self.dump(best_chunk_len)


