#!/bin/sh
import sys
import hashlib
import subprocess

# copy signature section to .sign file
subprocess.run(["objcopy","--dump-section",".sha=" + sys.argv[1] + ".sign", sys.argv[1]])

# remove .sha section from the binary into a copy_file
subprocess.run(["objcopy", "--remove-section=.sha", sys.argv[1], "copy_file"])

# Verify the binary
ret = subprocess.run(["openssl", "dgst", "-sha256",  "-verify",
                    sys.argv[2], "-signature", sys.argv[1] + ".sign", "-binary", "copy_file"], capture_output = True)

if (ret.stdout.find(b"OK") != -1):
    subprocess.run(["./" + sys.argv[1]])
else:
    print("Could not verify the binary!")

