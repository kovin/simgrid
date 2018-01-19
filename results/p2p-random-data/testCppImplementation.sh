#!/bin/bash

for i in 25 50 75 100 125 150 175 200 225 250 275 300;
    do
    echo ""
    echo "---------------------------------------------------------------"
    echo ""
    echo "Results for $i nodes in the p2p-random-data experiment for CPP (S4U)";
    sudo ../../utils/runAndReturnRssAndTime.sh \
    ../../examples/s4u/p2p-random-data/s4u-p2p-random-data \
    ../exponential_$[i]_nodes_.5_alpha.xml \
    $[i]_nodes_deployment_for_c.xml
    echo ""
    echo "---------------------------------------------------------------"
    echo ""
done
