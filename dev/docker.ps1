if ($args[0] -eq "build") {
    $confirmation = Read-Host "Are you sure you want to proceed? (Y/n)"
    if ($confirmation -ne 'y') {
        echo "Goodbye!"
        return
    }

    echo "Rebuilding docker image 'wroot'..."
    docker build -t wroot ../
} elseif ($args[0] -eq "run") {
    docker run -it -p 8000:8000/tcp -v G:\Programacao\wroot:/wroot wroot bash -c "cd /wroot/dev && ./run.sh"
} else {
    docker run -it -p 8000:8000/tcp -v G:\Programacao\wroot:/wroot wroot bash
}