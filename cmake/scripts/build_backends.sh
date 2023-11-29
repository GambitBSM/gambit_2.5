# Usage: build_backends.sh -f <input file> -s <list of backends to skip> -j <number of cores>

prefix="build_backends.sh:"
make_j_option=1

# Parse arguments
while getopts "f:s:j:" opt; do
  case $opt in
    f) backends_list_file="${OPTARG}"
    ;;
    s) skip_backends="${OPTARG}"
    ;;
    j) make_j_option="${OPTARG}"
    ;;
    \?) printf "${prefix} Invalid option. Valid options: -f <input file>  -s <list of backends to skip>  -j <number of cores>\n" >&2
    exit 1
    ;;
  esac

  case $OPTARG in
    -*) printf "${prefix} Option $opt needs a valid argument \n" >&2
    exit 1
    ;;
  esac
done

if [ -z ${backends_list_file} ];
then
  printf "${prefix} Need an input file. Example: build_backends.sh -f <input file>\n" >&2
  exit 1
fi

# Check that the input file exists
if [ ! -f ${backends_list_file} ];
then
  printf "${prefix} Cannot find the file ${backends_list_file}\n" >&2
  exit 1
fi

# For each backend listed in the input file:
# - Check if the backend is in the $skip_backends list.
# - If not, try to build it.
# - If the build fails, add it to $failed_backends_list
did_fail=0
failed_backends_list=""
while read backend || [[ -n $backend ]];
do
  do_skip_backend=0
  for skip_backend_name in $(echo ${skip_backends}); 
  do 
    if [ ${skip_backend_name} = ${backend} ];
    then
      do_skip_backend=1
    fi
  done

  if [ ${do_skip_backend} -eq 1 ];
  then
    printf "\n${prefix} Will skip backend ${backend}.\n"
  else
    printf "\n${prefix} Will now attempt to build backend ${backend}.\n"

    # Now run the build command
    make -j${make_j_option} ${backend}
    if [ $? -ne 0 ];
    then 
      did_fail=1
      failed_backends_list="${failed_backends_list}, ${backend}"
    fi
  fi
done < ${backends_list_file}

failed_backends_list="${failed_backends_list:2}"

# Report result
if [ ${did_fail} -eq 1 ];
then
  printf "\n${prefix} The following backends failed to build: ${failed_backends_list}\n" >&2
else
  printf "\n${prefix} All backends built successfully!\n"
fi

# Return correct exit code
exit ${did_fail}

