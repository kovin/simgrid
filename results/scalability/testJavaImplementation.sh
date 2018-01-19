#!/bin/bash

for i in 25 50 75 100 125 150 175 200 225 250 275 300;
    do
    echo ""
    echo "------------------------------------------------------------------"
    echo ""
    echo "Results for $i nodes in the app-scalability experiment for Java (bindings for MSG)";
    sudo ../../utils/runAndReturnRssAndTime.sh \
    java -cp ../../examples/java/:../../simgrid.jar \
    app.scalability.Main \
    ../exponential_$[i]_nodes_.5_alpha.xml \
    $[i]_nodes_deployment_for_java.xml
    echo ""
    echo "------------------------------------------------------------------"
    echo ""
done
