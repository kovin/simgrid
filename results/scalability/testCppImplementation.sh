#!/bin/bash

for i in 25 50 75 100 125 150 175 200 225 250 275 300;
    do
    echo ""
    echo "---------------------------------------------------------------"
    echo ""
    echo "Results for $i nodes in the app-scalability experiment for CPP (S4U)";
    sudo /home/kevin/exactas/simgrid/utils/runAndReturnRssAndTime.sh \
    /home/kevin/exactas/simgrid/examples/s4u/scalability/s4u-scalability \
    /home/kevin/exactas/simgrid/results/scalability/exponential_$[i]_nodes_.5_alpha.xml \
    /home/kevin/exactas/simgrid/results/scalability/$[i]_nodes_deployment_for_c.xml
    echo ""
    echo "---------------------------------------------------------------"
    echo ""
done
