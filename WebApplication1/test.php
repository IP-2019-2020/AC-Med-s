<?php
    echo '<script type="text/JavaScript">
    
    
     $config = {
     apiKey: "AIzaSyAKIzNv6jX6PklH5Uv6B2HNvbH5vEOi1nk",
     authDomain: "medicalapp-36d7a.firebaseapp.com",
     databaseURL: "https://medicalapp-36d7a.firebaseio.com",
     projectId: "medicalapp-36d7a",
     storageBucket: "medicalapp-36d7a.appspot.com",
     messagingSenderId: "492323991988",
     appId: "1:492323991988:web:c8b2f61ccbd94a360242e1",
     measurementId: "G-L9RLTV5WS2"
     };
     firebase.initializeApp(config);
   
     // Get a reference to the database service
     var database = firebase.database();
   
     function writeStaff(nume, cod, functie, utilizator, parola) {
     firebase.database().ref('Staff/' + nume).set({
       cod_id: cod,
       functie: functie,
       utilizator: utilizator,
       parola: parola
     });
   
     function postStaff()
     {
       if(isset($_POST['save_data']))
     {
        $cod=$_POST['cod'];
        $functie=$_POST['functie'];
        $nume=$_POST['nume'];
        $utilizator=$_POST['utilizator'];
        $parola=$_POST['parola'];
   
      }
   
      writeStaff($nume,$cod,$functie,$utilizator,$parola);
   
     }
   }
   
   </script>'
   ;
   
 ?>


