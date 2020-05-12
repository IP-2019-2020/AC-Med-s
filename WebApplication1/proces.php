<?php
 session_start()
include('dbconfig.php');

 if(isset($_POST['save_data']))
  {
     $cod=$_POST['cod'];
     $functie=$_POST['functie'];
     $nume=$_POST['nume'];
     $utilizator=$_POST['utilizator'];
     $parola=$_POST['parola'];

   }

 

   $data=[
            'cod' => $cod,
            'functie' => $functie,
            'nume' => $nume,
            'utilizator' => $utilizator,
            'parola' => $parola
          ];

          $ref='Staff/'

         $postdata = $database->getReference($ref)->push($data);

            if($postdata)
            {
              $_SESSION['statsu'] = "succesfully";
              header("Location")
            }


          


    ?>