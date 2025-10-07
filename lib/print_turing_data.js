function printName(name, length) {
    const lines = [
        "*".repeat(length),
        "*" + " ".repeat(length - 2) + "*",
        "*" + name.padStart((length + name.length - 2) / 2).padEnd(length - 2) + "*",
        "*" + " ".repeat(length - 2) + "*",
        "*".repeat(length)
    ];
    lines.forEach(line => console.log(line));
}

function printAlphabet(alphabet) {
    console.log("Alphabet : [", alphabet.join(", "), "]");
}

function printStates(states) {
    console.log("States : [", states.join(", "), "]");
}

function printInitial(initial) {
    console.log("Initial :", initial);
}

function printFinals(finals) {
    console.log("Finals : [", finals.join(", "), "]");
}

const printTransitions = transitions =>
    Object.entries(transitions).forEach(([state, tra]) =>
        tra.forEach(t =>
            console.log(`(${state}, ${t.read}) -> (${t.to_state}, ${t.write}, ${t.action})`)
        )
    );

function printTuringOperations(history, index = 0) {
    if (index !== history.length) {
        console.log(`[${history[index][0]}] ${history[index][1]}`);
        printTuringOperations(history, index + 1);
    }
}

export function printTuringData(turingData, history) {
    const length = turingData.name.length > 56 ? turingData.name.length + 4 : 60;
    printName(turingData.name, length);
    printAlphabet(turingData.alphabet);
    printStates(turingData.states);
    printInitial(turingData.initial);
    printFinals(turingData.finals);
    printTransitions(turingData.transitions);
    console.log("*".repeat(length));
    printTuringOperations(history.map(elm => [elm[0] + turingData.blank.repeat(10), elm[1]]));
}
