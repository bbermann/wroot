if ($args[0] -eq "build") {
    $confirmation = Read-Host "Are you sure you want to proceed? (Y/n)"
    if ($confirmation -ne 'y') {
        echo "Goodbye!"
        return
    }

    echo "Rebuilding docker image 'wroot'..."
    docker build -t wroot ../
} elseif ($args[0] -eq "run") {
    echo "Running wroot server..."
    docker run -d -p 8000:8000/tcp -p 666:666/tcp -p 22:22/tcp -v G:\Programacao\wroot:/wroot wroot bash -c "cd /wroot/dev && ./run.sh"
} elseif ($args[0] -eq "ssh") {
    echo "Running ssh into wroot container..."
    ssh root@localhost
} elseif ($args[0] -eq "stop") {
    echo "Stopping wroot container..."
    docker stop $(docker ps -q --filter ancestor=wroot)
} else {
    echo "Running wroot container"
    docker run -d -p 8000:8000/tcp -p 666:666/tcp -p 22:22/tcp -v G:\Programacao\wroot:/wroot wroot
    ssh root@localhost
}