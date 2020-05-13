const codId = document.getElementById('codId');
const functieId = document.getElementById('functieId');
const numeId = document.getElementById('numeId');
const utilizatorId = document.getElementById('utilizatorId');
const parolaId = document.getElementById('parolaId');
const send_button = document.getElementById('save_button');

const database = firebase.database();
const rootRef = database.ref('Staff');
var ref = database.ref('Staff/Marian');


function getData()
{
    ref.once('value').then(function(snap){
        var name = snap.val().Username;
        console.log(name);
    })
}


send_button.addEventListener('click', (e) => {
    e.preventDefault();
    rootRef.child(numeId.value).set({
        Cod_user: codId.value,
        Functie: functieId.value,
        Username: utilizatorId.value,
        Password: parolaId.value
    });  
});
