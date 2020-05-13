

function getData() {
    var firebaseUrl = "https://medicalapp-36d7a.firebaseio.com/";
    var base = FirebaseApp.getDatabaseByUrl(firebaseUrl);
    var username = base.getData("Staff/Marian/Username");
    alert(username);
}