#!/bin/bash

for i in 25 50 75 100 125 150 175 200 225 250 275 300;
    do
    echo ""
    echo "------------------------------------------------------------------"
    echo ""
    echo "Results for $i nodes in the app-scalability experiment for Java";
    sudo /home/kevin/exactas/simgrid/utils/runAndReturnRssAndTime.sh \
    java -cp /home/kevin/exactas/simgrid/examples/java/:/home/kevin/exactas/simgrid/simgrid.jar \
    app.scalability.Main \
    /home/kevin/exactas/simgrid/results/scalability/exponential_$[i]_nodes_.5_alpha.xml \
    /home/kevin/exactas/simgrid/results/scalability/$[i]_nodes_deployment_for_java.xml
    echo ""
    echo "------------------------------------------------------------------"
    echo ""
done
