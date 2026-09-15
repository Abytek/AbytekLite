/**
 * EventDrivenObject.js
 * 
 * Provides a factory for creating event-driven objects with a simple event system.
 * This is a core utility used throughout the Abytek build scripts to implement
 * the observer pattern and event-based communication.
 */
module.exports = (Module) => {
    /**
     * Creates an event-driven object with event handling capabilities
     * 
     * @param {Array<string>} [EventNames] - Optional list of event names to initialize
     * @returns {Object} An object with event handling methods
     */
    return function F_EventDrivenObject(EventNames)
    {
        let F_Object = Module.ImportScript("Abytek/Utilities/Object.js");
        let F_Event = Module.ImportScript("Abytek/Utilities/Event.js");

        return F_Object()
            .AddMethod(
                /**
                 * Connects a listener to a named event
                 * 
                 * @param {string} EventName - The name of the event to connect to
                 * @param {Function} Listener - The function to call when the event is triggered
                 * @returns {number} Handle to the connected listener (used for disconnecting)
                 */
                function Connect(EventName, Listener)
                {
                    if (!(EventName in this.Events))
                    {
                        throw new Error(`Not found event "${EventName}"`);
                    }
                    return this.Events[EventName].AddListener(Listener.bind(this));
                }
            )
            .AddMethod(
                /**
                 * Disconnects a listener from a named event
                 * 
                 * @param {string} EventName - The name of the event
                 * @param {number} ListenerHandle - The handle returned from Connect
                 */
                function Disconnect(EventName, ListenerHandle)
                {
                    if (!(EventName in this.Events))
                    {
                        throw new Error(`Not found event "${EventName}"`);
                    }
                    this.Events[EventName].RemoveListener(ListenerHandle);
                }
            )
            .AddMethod(
                /**
                 * Shorthand method to connect a listener to an event
                 * Supports two calling conventions:
                 * - On(eventName, listenerFunction)
                 * - On(listenerFunction) - uses function name as event name
                 * 
                 * @param {string|Function} EventName - Event name or listener function
                 * @param {Function} [Listener] - Listener function if first param is event name
                 * @returns {Object} This object for chaining
                 */
                function On(EventName, Listener)
                {
                    if (Listener)
                    {
                        this.Connect(EventName, Listener);
                    }
                    else
                    {
                        this.Connect(EventName.name, EventName);
                    }
                    return this;
                }
            )
            .AddMethod(
                /**
                 * Broadcasts an event to all connected listeners
                 * 
                 * @param {string} EventName - The name of the event to broadcast
                 * @param {...*} Args - Arguments to pass to the listeners
                 * @returns {Object} This object for chaining
                 */
                function Broadcast(EventName, ...Args)
                {
                    if (!(EventName in this.Events))
                    {
                        throw new Error(`Not found event "${EventName}"`);
                    }
                    this.Events[EventName].Broadcast(...Args);
                    return this;
                }
            )
            .AddMethod(
                /**
                 * Adds a new event to the object
                 * Also creates a method with the same name as the event that broadcasts the event
                 * 
                 * @param {string} EventName - The name of the event to add
                 * @returns {Object} This object for chaining
                 */
                function AddEvent(EventName)
                {
                    this.Events[EventName] = F_Event();
                    this[EventName] = function (...Args)
                    {
                        this.Events[EventName].Broadcast(...Args);
                        return this;
                    }.bind(this);
                    return this;
                }
            )
            .AddMethod(
                /**
                 * Adds an event and connects a base listener to it
                 * Supports two calling conventions:
                 * - AddEventMethod(eventName, listenerFunction)
                 * - AddEventMethod(listenerFunction) - uses function name as event name
                 * 
                 * @param {string|Function} EventName - Event name or listener function
                 * @param {Function} [EventBaseListener] - Listener function if first param is event name
                 * @returns {Object} This object for chaining
                 */
                function AddEventMethod(EventName, EventBaseListener)
                {
                    if (EventBaseListener)
                    {
                        this.AddEvent(EventName);
                        this.Connect(
                            EventName, 
                            EventBaseListener
                        );
                    }
                    else
                    {
                        this.AddEvent(EventName.name);
                        this.Connect(
                            EventName.name,
                            EventName
                        );
                    }
                    return this;
                }
            )
            (
                /**
                 * Constructor function for the event-driven object
                 * Initializes the events collection and adds any specified events
                 */
                function ()
                {
                    this.Events = new Object();
                    if (EventNames)
                    {
                        for (let EventName of EventNames)
                        {
                            this.AddEvent(EventName);
                        }
                    }
                }
            );
    };
}