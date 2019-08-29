#
# Setup script for the Simba development environment.
#

if [ "${OSTYPE}" = "linux-gnu" ]; then
    source environment/setup_linux.sh
else
    echo "\$\{OSTYPE\} must be linux-gnu."
    exit 1
fi
