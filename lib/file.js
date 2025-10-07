export async function readJsonFile(filePath) {
    try {
        const rawData = await Deno.readTextFile(filePath);
        return JSON.parse(rawData);
    } catch (error) {
        if (error instanceof Deno.errors.NotFound) {
            throw new Error(`File not found: ${filePath}`);
        }
        if (error instanceof SyntaxError) {
            throw new Error(`Invalid JSON in file: ${filePath}`);
        }
        throw new Error(`Failed to read file: ${error.message}`);
    }
}
