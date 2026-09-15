/**
 * List.js
 * 
 * Provides a simple doubly-linked list implementation.
 * This utility is used throughout the Abytek build system for managing
 * collections of items with efficient insertion and removal operations.
 */
module.exports = (Module) => {
    /**
     * Factory function for creating linked list objects
     * 
     * @param {...*} [Args] - Arguments passed to the list constructor
     * @returns {F_List} A new linked list object
     */
    return function (...Args)
    {
        /**
         * Class representing a doubly-linked list
         */
        class F_List
        {
            /**
             * Creates a new linked list with optional default elements
             * 
             * @param {Array} [DefaultElements] - Initial elements to add to the list
             */
            constructor(DefaultElements)
            {
                if (DefaultElements)
                {
                    for (let DefaultElement of DefaultElements)
                    {
                        this.Add(DefaultElement);
                    }
                }
                this.HeadNode = null;
                this.TailNode = null;
            }

            /**
             * Adds an element to the end of the list
             * 
             * @param {*} Element - The element to add
             * @returns {Object} The node containing the added element
             */
            Add(Element)
            {
                let Node = {
                    Value: Element,
                    Next: null,
                    Prev: this.TailNode,
                }
                if (this.TailNode)
                {
                    this.TailNode.Next = Node;
                }
                else
                {
                    this.HeadNode = Node;
                    this.TailNode = Node;
                }
                return Node;
            }

            /**
             * Removes a node from the list
             * 
             * @param {Object} Node - The node to remove (returned by Add)
             */
            Remove(Node)
            {
                if (Node.Prev)
                {
                    Node.Prev.Next = Node.Next;
                }
                else
                {
                    this.HeadNode = Node.Next;
                }
                if (Node.Next)
                {
                    Node.Next.Prev = Node.Prev;
                }
                else
                {
                    this.TailNode = Node.Prev;
                }
            }
            
            /**
             * Returns the first node in the list for iteration
             * 
             * @returns {Object|null} The first node or null if the list is empty
             */
            Begin()
            {
                return this.HeadNode;
            }

            /**
             * Returns the end marker for list iteration (always null)
             * 
             * @returns {null} Always returns null
             */
            End()
            {
                return null;
            }
        }
        return new F_List(...Args);
    };
}