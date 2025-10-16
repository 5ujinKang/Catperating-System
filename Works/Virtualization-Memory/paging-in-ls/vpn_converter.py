import sys

PAGE_SIZE = 4096
OFFSET_BITS = 12

def addr_to_vpn(addr):
    return addr >> OFFSET_BITS

def process_trace_file(input_file, output_file):
    with open(input_file, 'r') as f_in, open(output_file, 'w') as f_out:
        for line in f_in:
            line = line.strip()
            if not line or line.startswith("=="):
                continue
            try:
                parts = line.split()
                if len(parts) < 2:
                    continue
                addr_str = parts[1].split(',')[0]
                addr = int(addr_str, 16)
                vpn = addr_to_vpn(addr)
                f_out.write(f"{vpn}\n")
            except:
                continue

if len(sys.argv) < 3:
    print("Usage: python vpn_converter.py trace.txt output.txt")
    sys.exit(1)

process_trace_file(sys.argv[1], sys.argv[2])