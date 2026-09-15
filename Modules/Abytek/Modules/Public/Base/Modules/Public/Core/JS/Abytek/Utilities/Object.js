/**
 * Object.js
 * 
 * Provides a foundational factory for creating extensible objects with method chaining.
 * This is a core utility used throughout the Abytek build system as a base for
 * creating more specialized objects with fluent interfaces.
 */
module.exports = (Module) => {
    /**
     * Creates a base object with method chaining support
     * 
     * @returns {Function} An object-like function that supports method chaining
     */
    return function F_Object()
    {
        /**
         * The result object is actually a function that allows executing a callback
         * in its own context and supports method chaining
         * 
         * @param {Function} Callback - A function to execute in the context of this object
         * @returns {Object} This object for chaining
         */
        var Result = function ExecuteOnThis(Callback)
        {
            if (Callback)
            {
                Callback.bind(Result)();
            }
            return Result;
        };

        /**
         * Adds a method to the object
         * Supports two calling conventions:
         * - AddMethod(name, function) - adds function with the given name
         * - AddMethod(function) - adds function using its name property
         * 
         * @param {string|Function} Name - Method name or function
         * @param {Function} [Callback] - Function to add if first param is name
         * @returns {Object} This object for chaining
         */
        Result.AddMethod = function AddMethod(Name, Callback) {
            if (Callback)
            {
                this[Name] = Callback.bind(this);
            }
            else
            {
                this[Name.name] = Name;
            }
            return this; 
        }.bind(Result);
        
        return Result;
    };
}