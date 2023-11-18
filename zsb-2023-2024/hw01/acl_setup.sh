#!/bin/bash

echo "Enter username: "
read username
echo "Enter directory (public or private): "
read directory
echo "Enter rights: (r, w, or rw): "
read rights

setfacl -m "u:$username:$rights" "$directory"
echo "ACL set for $user on $directory with $rights done"
 