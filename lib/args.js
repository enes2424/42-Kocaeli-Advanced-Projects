export function parseArgs() {
  const args = Deno.args;
  const num_of_lib_files = 5;
  const length = args.length - num_of_lib_files;

  if (length === 0) throw new Error("Argument missing. Please provide arguments.");
  if (length === 1 && (args[num_of_lib_files] === "--help" || args[num_of_lib_files] === "-h")) printHelp();
  if (length === 1) throw new Error("Expected 2 arguments, got 1.");
  if (length > 2) throw new Error("Too many arguments. Expected 2.");

  return {
    jsonFile: args[num_of_lib_files],
    input: args[num_of_lib_files + 1]
  };
}

function printHelp() {
  console.log(`
    Turing Machine Simulator

    usage: ft_turing [-h] jsonfile input

    positional arguments:
      jsonfile\tjson description of the machine
      input\tinput of the machine

    optional arguments:
      -h, --help\tshow this help message and exit`);
  Deno.exit(0);
}
