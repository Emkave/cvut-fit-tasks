#!/bin/bash

echo "Enter file: "
read file_name
chmod g-r "$file_name"
echo "Read permission revoked for $file_name"
