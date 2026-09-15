/**
 * Event.js
 * 
 * Provides a simple event implementation for the observer pattern.
 * Events allow registering multiple listeners that can be notified when the event is broadcast.
 * This is a core utility used throughout the Abytek build system for event-driven architecture.
 */
module.exports = (Module) => {
    /**
     * Factory function for creating event objects
     * 
     * @param {...Function} [Args] - Default listeners to add to the event
     * @returns {F_Event} A new event object
     */
    return function (...Args)
    {
        let F_LinkedList = Module.ImportScript("Abytek/Utilities/List.js");
        
        /**
         * Class representing an event with listeners
         */
        class F_Event
        {
            /**
             * Creates a new event with optional default listeners
             * 
             * @param {Array<Function>} [DefaultListeners] - Initial listeners to register
             */
            constructor(DefaultListeners)
            {
                this.Listeners = F_LinkedList(DefaultListeners);
            }
            
            /**
             * Adds a listener to the event
             * 
             * @param {Function} Listener - The function to call when the event is broadcast
             * @returns {number} Handle used to identify the listener for removal
             */
            AddListener(Listener)
            {
                this.Listeners.Add(Listener);
            }

            /**
             * Removes a listener from the event
             * 
             * @param {number} ListenerHandle - The handle returned by AddListener
             */
            RemoveListener(ListenerHandle)
            {
                this.Listeners.Remove(ListenerHandle);
            }

            /**
             * Broadcasts the event to all registered listeners
             * 
             * @param {...*} Args - Arguments to pass to each listener
             */
            Broadcast(...Args)
            {
                for (let It = this.Listeners.Begin(); It !== this.Listeners.End(); It = It.Next)
                {
                    It.Value(...Args);
                }
            }
        }
        return new F_Event(...Args);
    };
}