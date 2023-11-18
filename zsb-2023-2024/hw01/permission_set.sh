#!/bin/bash

echo "Enter file: "
read file_name
chmod 4 "$file_name"
echo "Read permission granted for $file_name"
