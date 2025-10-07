export function validateMachine(machineData) {
    validateRequiredFields(machineData);
    validateAlphabet(machineData.alphabet, machineData.blank);
    validateStates(machineData.states, machineData.initial, machineData.finals);
    validateTransitions(machineData.transitions, machineData.finals, machineData.states, machineData.alphabet);
}

export function validateInput(input, alphabet, blank) {
    if (input.includes(blank))
        throw new Error(`Input contains blank character '${blank}' which is not allowed`);

    const invalidChars = [...input].filter(char => !alphabet.includes(char));
    if (invalidChars.length > 0)
        throw new Error(`Input contains characters not in alphabet: ${invalidChars.join(', ')}`);
}

function validateRequiredFields(machineData) {
    const requiredFields = ['name', 'alphabet', 'blank', 'states', 'initial', 'finals', 'transitions'];
    const missingFields = requiredFields.filter(field => !(field in machineData));

    if (missingFields.length > 0)
        throw new Error(`Missing required fields: ${missingFields.map(f => `'${f}'`).join(', ')}`);
}

function validateAlphabet(alphabet, blank) {
    if (!Array.isArray(alphabet))
        throw new Error("Alphabet must be an array");

    if (alphabet.length === 0)
        throw new Error("Alphabet cannot be empty");

    const invalidChars = alphabet.filter(char => typeof char !== 'string' || char.length !== 1);
    if (invalidChars.length > 0)
        throw new Error(`Alphabet contains invalid characters: ${invalidChars.join(', ')}. Each must be a single character string`);

    if (typeof blank !== 'string' || blank.length !== 1)
        throw new Error("Blank character must be a single character");

    if (!alphabet.includes(blank))
        throw new Error(`Blank character '${blank}' must be in alphabet`);
}

function validateStates(states, initial, finals) {
    if (!Array.isArray(states))
        throw new Error("States must be an array");

    if (states.length === 0)
        throw new Error("States list cannot be empty");

    if (new Set(states).size !== states.length)
        throw new Error("States list contains duplicate values");

    if (!states.includes(initial))
        throw new Error(`Initial state '${initial}' not found in states list`);

    if (!Array.isArray(finals))
        throw new Error("Finals must be an array");

    const invalidFinals = finals.filter(state => !states.includes(state));
    if (invalidFinals.length > 0)
        throw new Error(`Final states not found in states list: ${invalidFinals.join(', ')}`);
}

function validateTransitions(transitions, finals, states, alphabet) {
    if (typeof transitions !== 'object' || transitions === null)
        throw new Error("Transitions must be an object");

    const stateList = Object.keys(transitions);

    const missingStates = states.filter(state => !(stateList.includes(state)));

    const finalsInTransitions = finals.filter(state => !(missingStates.includes(state)));
    if (finalsInTransitions.length > 0)
        throw new Error(`Finals must not be in transitions: ${finalsInTransitions.join(', ')}`);

    const missingStatesWithoutFinals = missingStates.filter(state => !(finals.includes(state)));

    if (missingStatesWithoutFinals.length > 0)
        throw new Error(`Missing transitions for states: ${missingStatesWithoutFinals.join(', ')}`);

    Object.entries(transitions).forEach(([state, stateTransitions]) => {
        validateTransitionState(state, states);
        validateStateTransitions(state, stateTransitions, states, alphabet);
    });
}

function validateTransitionState(state, states) {
    if (!states.includes(state))
        throw new Error(`Transition state '${state}' not found in states list`);
}

function validateStateTransitions(state, transitions, states, alphabet) {
    if (!Array.isArray(transitions))
        throw new Error(`Transitions for state '${state}' must be an array`);

    if (transitions.length === 0)
        throw new Error(`State '${state}' has no transitions defined`);

    const seenChars = new Set();
    transitions.forEach(transition => {
        validateTransitionObject(transition, state);
        validateTransitionFields(transition, state, states, alphabet);
        checkForDuplicateReads(transition, state, seenChars);
    });
}

function validateTransitionObject(transition, state) {
    if (typeof transition !== 'object')
        throw new Error(`Invalid transition format in state '${state}'`);
}

function validateTransitionFields(transition, state, states, alphabet) {
    const requiredFields = ['read', 'to_state', 'write', 'action'];
    const missingFields = requiredFields.filter(field => !(field in transition));
    if (missingFields.length > 0)
        throw new Error(`Missing fields in transition for state '${state}': ${missingFields.join(', ')}`);

    if (!alphabet.includes(transition.read))
        throw new Error(`Invalid read character '${transition.read}' in state '${state}'`);

    if (!alphabet.includes(transition.write))
        throw new Error(`Invalid write character '${transition.write}' in state '${state}'`);

    if (!states.includes(transition.to_state))
        throw new Error(`Invalid to_state '${transition.to_state}' in state '${state}'`);

    if (!['LEFT', 'RIGHT'].includes(transition.action))
        throw new Error(`Invalid action '${transition.action}' in state '${state}'. Must be LEFT or RIGHT`);
}

function checkForDuplicateReads(transition, state, seenChars) {
    if (seenChars.has(transition.read))
        throw new Error(`Duplicate transition for character '${transition.read}' in state '${state}'`);

    seenChars.add(transition.read);
}