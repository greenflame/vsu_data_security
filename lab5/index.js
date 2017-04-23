let _ = require('lodash');
let fs = require('fs');
let readlineSync = require('readline-sync');

function generate_freq_dict(text) {
    return _(text.split(''))
        .countBy()
        .map((v, k) => { return { chr: k, cnt: v }; })
        .filter(t => t.chr.toLowerCase() != t.chr.toUpperCase())
        .orderBy('cnt', 'desc')
        .value();
}

function generate_substitution(dict_learn, dict_decode) {
    let subst = {};

    for (let i = 0; i < Math.min(dict_learn.length, dict_decode.length); i++) {
        subst[dict_decode[i].chr] = dict_learn[i].chr;
    }

    return subst;
}

function apply_substitution(text, subst){
    return _(text.split(''))
        .map(c => _.has(subst, c) ? subst[c] : c)
        .value()
        .join('');
}

function main() {
    let text_learn = fs
        .readFileSync('english_learn.txt', 'utf-8')
        .toLowerCase();

    let text_decode = fs
        .readFileSync('english_decode.txt', 'utf-8')
        .toLowerCase();

    let dict_learn = generate_freq_dict(text_learn);
    let dict_decode = generate_freq_dict(text_decode);
    
    let subst = generate_substitution(dict_learn, dict_decode);
    let decoded_text = apply_substitution(text_decode, subst);

    console.log(decoded_text);

    while (true) {
        let cmd = readlineSync.question('subst: ');
        if (cmd === 'x') {
            break;
        }

        if (cmd.length === 2) {
            decoded_text = decoded_text
                .split('')
                .map(i => {
                    if (i === cmd[0]) return cmd[1];
                    if (i === cmd[1]) return cmd[0];
                    return i;
                })
                .join('');
            
            console.log(decoded_text);
        }
    }
}

main();