let fs = require('fs');
let _ = require('lodash');


function makeTransition(machine) {
    let currentRule = _(machine.rules)
        .filter(r => machine.state === r[0] && machine.tape[machine.pos] === r[1])
        .first();

    if (typeof(currentRule) === 'undefined') {
        return false;
    }

    machine.state = currentRule[2];
    machine.tape[machine.pos] = currentRule[3];

    switch (currentRule[4])
    {
        case '>':
            machine.pos++;
            break;
        case '<':
            machine.pos--;
            break;
        case '-':
            break;
    }

    return true;
}

function main() {
    let machine = JSON.parse(fs.readFileSync('turing.json', 'utf-8'));
    
    do{
        console.log(machine.tape.join(' '));

        let pointerTape = _(machine.tape)
            .map((c, i) => machine.pos === i ? '^' : ' ')

        console.log(pointerTape.join(' ') + ' state: ' + machine.state);

    } while (makeTransition(machine))
}

main();
