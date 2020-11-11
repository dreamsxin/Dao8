<?php

namespace Dao\Async;

var_dump(Signal::isSupported(Signal::SIGINT));
var_dump(Signal::isSupported(Signal::SIGUSR1));

$signal = new Signal(Signal::SIGINT);

echo "Listening for SIGINT, press CTRL + C to terminate...\n";

$signal->awaitSignal();

echo "\n=> Signal received :)\n";
