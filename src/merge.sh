for file in *.cpp *.h; do echo "=== $file ==="; cat "$file"; echo ""; done > output.txt
