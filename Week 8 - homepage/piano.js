document.addEventListener('DOMContentLoaded', function(){

    let keys = document.querySelectorAll('button');

    notes = loadSounds();



    for(let i = 0; i < keys.length; i++)
    {
    keys[i].addEventListener('click', function(){


        switch(keys[i].id)
        {
            case 'c2':
                notes[0].play();
                break;
            case 'cs2':
                notes[1].play();
                break;
            case 'd2':
                notes[2].play();
                break;
            case 'ds2':
                notes[3].play();
                break;
            case 'e2':
                notes[4].play();
                break;
            case 'f2':
                notes[5].play();
                break;
            case 'fs2':
                notes[6].play();
                break;
            case 'g2':
                notes[7].play();
                break;
            case 'gs2':
                notes[8].play();
                break;
            case 'a2':
                notes[9].play();
                break;
            case 'as2':
                notes[10].play();
                break;
            case 'b2':
                notes[11].play();
                break;
            case 'c3':
                notes[12].play();
                break;
            default:
                break;
        }

    });
    }



});


function loadSounds()
{
    //this seems redundant; is there a better way to load all of these from a loop or something?

    let notes = [];
    notes[0] = new Howl({src: ['/sounds/C2.wav'], volume: 1.0});
    notes[1] = new Howl({src: ['/sounds/CS2.wav'], volume: 1.0});
    notes[2] = new Howl({src: ['/sounds/D2.wav'], volume: 1.0});
    notes[3] = new Howl({src: ['/sounds/DS2.wav'], volume: 1.0});
    notes[4] = new Howl({src: ['/sounds/E2.wav'], volume: 1.0});
    notes[5] = new Howl({src: ['/sounds/F2.wav'], volume: 1.0});
    notes[6] = new Howl({src: ['/sounds/FS2.wav'], volume: 1.0});
    notes[7] = new Howl({src: ['/sounds/G2.wav'], volume: 1.0});
    notes[8] = new Howl({src: ['/sounds/GS2.wav'], volume: 1.0});
    notes[9] = new Howl({src: ['/sounds/A2.wav'], volume: 1.0});
    notes[10] = new Howl({src: ['/sounds/AS2.wav'], volume: 1.0});
    notes[11] = new Howl({src: ['/sounds/B2.wav'], volume: 1.0});
    notes[12] = new Howl({src: ['/sounds/C3.wav'], volume: 1.0});
    return notes;
}