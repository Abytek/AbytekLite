/**
 * CLIArgumentMap.js
 * 
 * Provides a utility for parsing command-line arguments into a structured
 * configuration object. This is used throughout the Abytek build system
 * for handling command-line interfaces.
 */
module.exports = (Module) => {
    /**
     * Parses command-line arguments into a structured configuration object
     * 
     * @param {Array<string>} CLIArgs - Array of command-line arguments
     * @param {Function} [Validator] - Optional validator function that gets called with (config, currentKey)
     * @returns {Object} Configuration object where keys are argument names without '--' prefix
     *                  and values are arrays of associated values
     * 
     * @example
     * // For args ["--output", "file.txt", "--verbose"]
     * // Returns { output: ["file.txt"], verbose: [] }
     */
    return (CLIArgs, Validator) => {
        let config = {};
        let currentKey = null;
        CLIArgs.forEach(arg => {
            if (arg.startsWith('--')) {
                currentKey = arg.replace(/^--/, '');
                config[currentKey] = [];
            } else if (currentKey) {
                let value = arg.replace(/^"(.*)"$/, '$1').replace(/\\"/g, '"');
                config[currentKey].push(value);
            }
            if (Validator && currentKey)
            {
                Validator(config, currentKey);
            }
        });
        return config;
    };
}