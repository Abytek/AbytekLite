/**
 * Responsible.js
 * 
 * Provides a factory for creating responsibility objects that handle responses.
 * This is used throughout the Abytek build system to enable callback-based 
 * response handling in a chainable manner.
 */
module.exports = (Module) => {
    /**
     * Factory function for creating responsibility objects
     * 
     * @param {Function} Callback - The callback function to execute when a response is received
     * @returns {Object} A responsible object with methods for handling responses
     */
    return function F_Responsible(Callback)
    {
        let F_Object = Module.ImportScript("Abytek/Utilities/Object.js");

        return F_Object()
            .AddMethod(
                /**
                 * Handles a single response value by passing it to the callback
                 * 
                 * @param {*} Value - The response value to process
                 * @returns {Object} This object for chaining
                 */
                function Response(Value)
                {
                    Callback(Value);
                    return this;
                }
            )
            .AddMethod(
                /**
                 * Handles multiple response values by iterating through them
                 * 
                 * @param {Array|Iterable} Range - Collection of values to process
                 * @returns {Object} This object for chaining
                 */
                function ResponseRange(Range)
                {
                    for (let Value of Range)
                    {
                        Callback(Value);
                    }
                    return this;
                }
            )
    }
};