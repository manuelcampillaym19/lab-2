#!/bin/bash
exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3

# Everything below will go to the file 'log':

target_file="list.c"
testcode="test.c"

# Verifica si se realizaron cambios
CHANGED=$(diff --ignore-space-at-eol -b -w -B $target_file log 2>/dev/null)

# Si hubo cambios o no existe el binario, se actualiza el log
if [ -n "$CHANGED" ] || [ ! -f "a.out" ]; then
  echo "$(date): " >> log
  diff --stat --ignore-space-at-eol -b -w -B $target_file log 2>/dev/null >> log

  # Compilation
  echo "Compiling with: gcc $testcode -Wall -Werror -o a.out" >&3
  if gcc $testcode -Wall -Werror -o a.out 2>>log ; then
      echo  " tests: " $(($(./a.out | grep -c 'OK')))\|$(($(./a.out | grep -c 'FAILED'))) >> log
      ./a.out | tail -n1 >> log
  else
      echo "Compilation with errors :c" >&3
      echo "Compilation failed" >> log
      gcc $testcode -Wall -Werror -o a.out 2>&3
      exit 1
  fi
fi

# Retroalimentación

if ./a.out &> /dev/null ; then
  echo "Executing: ./a.out" >&3
  ./a.out >&3
else
  echo "Errors in execution =O" >&3
  gcc -g $testcode -o a.out >&3
  gdb -silent -ex='set disable-randomization off' -ex='set confirm off' -ex='run' -ex=quit a.out >&3
fi

