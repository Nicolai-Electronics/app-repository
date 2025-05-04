#!/usr/bin/env bash

set -ue

for dir in ../*; do
    dir=${dir##*/}
    if [ -d "../$dir" ]; then
        echo "Validating metadata for app \"$dir\"..."
        node validate.js ../$dir/metadata.json
    fi
done

echo "All application metadata passes validation!"
