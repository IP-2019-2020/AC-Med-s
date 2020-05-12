<?php

require __DIR__.'/vendor/autoload.php';

use Kreaite\Firebase\Factory;
use Kreaite\Firebase\ServiceAccount;


$serviceAccount = ServiceAccount::fromJsonFile(__DIR__.'/medicalapp-36d7a-firebase-adminsdk-o3ni7-b34985e12e.json');

$firebase=(new Factory)

    ->withServiceAccount($serviceAccount)
    ->withDatabaseUrl('https://medicalapp-36d7a.firebaseio.com')
    ->create();


$database =$firebase->getDatabase();


?>