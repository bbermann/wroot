#!/bin/bash

function confirm()
{
    echo -n "$@ "
    read -e answer
    for response in y Y yes YES Yes Sure sure SURE OK ok Ok
    do
        if [ "_$answer" == "_$response" ]
        then
            return 0
        fi
    done

    # Any answer other than the list above is considerred a "no" answer
    return 1
}

if [[ $1 -eq "build" ]];
then
    confirm Rebuild docker image? && \
    echo "Rebuilding docker image 'wroot'..." && \
    docker build -t wroot ../
elif [[ $1 -eq "run" ]];
then
    echo "Running wroot server..."
    docker run -d -p 8000:8000/tcp -p 666:666/tcp -p 22:22/tcp -v G:\Programacao\wroot:/wroot wroot bash -c "cd /wroot/dev && ./run.sh"
elif [[ $1 -eq "ssh" ]];
then
    echo "Running ssh into wroot container..."
    ssh root@localhost
elif [[ $1 -eq "stop" ]];
then
    echo "Stopping wroot container..."
    docker stop $(docker ps -q --filter ancestor=wroot)
else
    echo "Running wroot container"
    docker run -d -p 8000:8000/tcp -p 666:666/tcp -p 22:22/tcp -v G:\Programacao\wroot:/wroot wroot
    ssh root@localhost
fi