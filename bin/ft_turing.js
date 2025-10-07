import { parseArgs } from '../lib/args.js';
import { readJsonFile } from '../lib/file.js';
import { validateMachine, validateInput } from '../lib/validation.js';
import { printTuringData } from '../lib/print_turing_data.js';
import { executeTuring } from '../lib/execute_turing.js';

async function main() {
    try {
        const args = parseArgs();
        const machineData = await readJsonFile(args.jsonFile);

        validateMachine(machineData);
        validateInput(args.input, machineData.alphabet, machineData.blank);
        const history = executeTuring(machineData, args.input, machineData.initial);
        printTuringData(machineData, history);
    } catch (error) {
        console.error("Error:", error.message);
        Deno.exit(1);
    }
}

main();