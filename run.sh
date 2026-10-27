#!/bin/bash
# ==============================================================================
# Script to Compile and Launch the Smart Agricultural By-Product Marketplace
# Platform: Ubuntu / Linux (VS Code Terminal)
# ==============================================================================

echo "=========================================================="
echo "🌱 Compiling Smart Agricultural By-Product Marketplace..."
echo "=========================================================="

# Compile with g++ (standard Linux sockets, C++17)
g++ -std=c++17 main.cpp -o marketplace

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Starting C++ Server on http://localhost:8080 ..."
    ./marketplace
else
    echo "Compilation failed! Ensure g++ is installed (sudo apt install g++)."
fi
