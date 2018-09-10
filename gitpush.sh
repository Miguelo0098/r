#!/bin/bash
if [[ $# -lt 1 ]]; then
	echo "Error: no se han introducido los argumentos necesarios"
	exit
fi
git add .
git commit -m "$1"
git push