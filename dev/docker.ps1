if ($args[0] -eq "build") {
    $confirmation = Read-Host "Are you sure you want to proceed? (Y/n)"
    if ($confirmation -ne 'y') {
        echo "Goodbye!"
        return
    }

    echo "Rebuilding docker image 'wroot'..."
    docker-compose build wroot-dev
} elseif ($args[0] -eq "ssh") {
    echo "Running ssh into wroot container..."
    ssh root@localhost
} elseif ($args[0] -eq "stop") {
    echo "Stopping wroot container..."
    docker-compose down wroot-dev
} else {
    echo "Running wroot container"
    docker-compose up -d wroot-dev
    ssh root@localhost
}