/**
 * CPPMethodArgsToString.js
 * 
 * Provides a utility for converting JavaScript method parameter definitions
 * to C++ parameter strings during code generation.
 */
module.exports = (Module) => {
    /**
     * Converts a JavaScript object representing method arguments to a C++ parameter string
     * 
     * @param {Object} Args - Object where keys are argument names and values contain DataType and optional DefaultValue
     * @returns {string} Formatted C++ parameter string
     * 
     * @example
     * // Returns "int count = 0, bool enabled = true"
     * CPPMethodArgsToString({
     *   count: { DataType: 'int', DefaultValue: '0' },
     *   enabled: { DataType: 'bool', DefaultValue: 'true' }
     * });
     */
    return (Args) => {
        let ArgList = '';
        for (let ArgName in Args)
        {
            if (ArgList !== '')
            {
                ArgList += ', \n';
            }
            let Arg = Args[ArgName];
            ArgList += `${Arg.DataType} ${ArgName} ${Arg.DefaultValue ? ` = ${Arg.DefaultValue}` : ''}`;
        }
        return ArgList;
    };
}