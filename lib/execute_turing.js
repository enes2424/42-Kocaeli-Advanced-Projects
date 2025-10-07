const replaceChar = (str, index, newChar) =>
    str.substring(0, index) + newChar + str.substring(index + 1);

const prepend = (str, char) => char + str;

const append = (str, char) => str + char;

const visualizeTape = (str, index, blank) => {
    if (index === -1) return `<${blank}>${str}`;
    if (index === str.length) return `${str}<${blank}>`;
    return `${str.substring(0, index)}<${str[index]}>${str.substring(index + 1)}`;
};

const describeTransition = (currentState, transition) =>
    `(${currentState}, ${transition.read}) -> (${transition.to_state}, ${transition.write}, ${transition.action})`;

export function executeTuring(turingData, input, state, index = 0, history = []) {
    if (turingData.finals.includes(state))  return history;

    const stateTransitions = turingData.transitions[state];

    const currentChar = index === -1 || index === input.length
        ? turingData.blank
        : input[index];

    const transition = stateTransitions.find(t => t.read === currentChar);
    if (!transition) {
        throw new Error(`No valid transition found for state '${state}' with input '${currentChar}'`);
    }

    const { to_state, write, action } = transition;

    const calculateNewState = () => {
        if (index === -1) {
            return {
                newInput: prepend(input, write),
                newIndex: action === "RIGHT" ? 1 : -1,
                newhistory: history.map(elm => [prepend(elm[0], turingData.blank), elm[1]])
            };
        }
        if (index === input.length) {
            return {
                newInput: append(input, write),
                newIndex: action === "RIGHT" ? index + 1 : index - 1,
                newhistory: history.map(elm => [append(elm[0], turingData.blank), elm[1]])
            };
        }
        return {
            newInput: replaceChar(input, index, write),
            newIndex: action === "RIGHT" ? index + 1 : index - 1,
            newhistory: history
        };
    };

    const { newInput, newIndex, newhistory } = calculateNewState();

    const tapeVisualization = visualizeTape(input, index, turingData.blank);
    const transitionDescription = describeTransition(state, transition);

    return executeTuring(
        turingData,
        newInput,
        to_state,
        newIndex,
        [...newhistory, [tapeVisualization, transitionDescription]]
    );
}