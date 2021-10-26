#!/bin/bash
qemu-system-x86_64 -cpu qemu64 -hda Image/x64BareBonesImage.qcow2 -m 512 -d int
