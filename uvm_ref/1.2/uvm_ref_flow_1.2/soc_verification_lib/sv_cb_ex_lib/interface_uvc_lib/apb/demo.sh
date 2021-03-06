#!/bin/sh

if [ ! -n "$UVM_HOME" ]; then  
   UVM_HOME=`ncroot`/tools/uvm-1.1
   export UVM_HOME               
fi                              

DEMO_HOME=`dirname $0`
TB_HOME=$DEMO_HOME/examples
TEST_NAME=test_read_after_write
#TEST_NAME=test_multiple_read_after_write

export TB_HOME

usage() {
    echo "Usage:  demo.sh [-test <test_file>]"
    echo "                [-batch]"
    echo ""
    echo "                ... all other Incisive Simulator/UVM options such as:"
    echo "                [+UVM_VERBOSITY=UVM_NONE | UVM_LOW | UVM_MEDIUM | UVM_HIGH | UVM_FULL]"
    echo "                [+UVM_SEVERITY=INFO | WARNING | ERROR | FATAL]"
    echo "                [-SVSEED  random | <integer-value>]"
    echo ""
    echo "        demo.sh -h[elp]"
    echo ""
}

# =============================================================================
# Get args
# =============================================================================
gui="-access rc -linedebug -gui +tcl+$TB_HOME/sim_command.tcl"
other_args="";

while [ $# -gt 0 ]; do
   case $1 in
      -h|-help)
                        usage
                        exit 1
                        ;;
      -batch)
                        gui=""
                        ;;
      *)
                        other_args="$other_args $1"
                        ;;
    esac
    shift
done

# =============================================================================
# Execute
# =============================================================================

irun -uvmhome $UVM_HOME \
  -incdir $DEMO_HOME/sv \
  -incdir $DEMO_HOME/examples \
  -coverage b:u \
  -covoverwrite \
  -nowarn PMBDVX \
  +UVM_TESTNAME=$TEST_NAME \
  +UVM_VERBOSITY=UVM_MEDIUM \
  $TB_HOME/demo_top.sv \
  $gui \
  $other_args
