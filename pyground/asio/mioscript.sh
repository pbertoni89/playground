#!/bin/bash

# assuming xnext as username
directory_python_tua="/path/a/dir/tua"
directory_python_deploy="/path/a/dir/su/ammiraglia"

scp -r $directory_python_tua ammiraglia:$directory_python_deploy

ssh ammiraglia "cd $directory_python_deploy ; ./mioscript.py"

