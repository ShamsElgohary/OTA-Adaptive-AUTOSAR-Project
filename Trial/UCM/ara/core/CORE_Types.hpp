#ifndef _ARA_CORE_CORE_TYPES_HPP_
#define _ARA_CORE_CORE_TYPES_HPP_



namespace ara
{
    namespace core
    {
        enum class future_status : unsigned char
        {
        ready, ///< the shared state is ready
        timeout, ///< the shared state did not become ready before the specified timeout has passed
        };

        template <typename T>
        class Future
        {
            public:
            
            Future() noexcept = default;
            
            ~Future();
            
            /* NO COPY CONSTRUCTOR AND ASSIGNMENT (NO COPYING) */
            Future(Future const&) = delete;
            Future& operator=(Future const&) = delete;

            /* MOVE SEMANTICS */     
            Future(Future&& other) noexcept;
            Future& operator=(Future&& other) noexcept;


            /* @returns value of type T
            @error Domain:error the error that has been put into the corresponding Promise via Promise::SetError*/
            T get();


            /*Checks if the Future is valid, i.e. if it has a shared state.
            * @returns true if the Future is usable, false otherwise  */
            bool valid() const noexcept;

            /* Wait for a value or an error to be available. */
            void wait() const;


            /*
            * @brief Return whether the asynchronous operation has finished.
            *
            * If this function returns true, get(), GetResult() and the wait calls
            are guaranteed not to block.
            *
            * @returns true if the Future contains a value or an error, false
            otherwise
            */
            bool is_ready() const;

            private:

            T value;
        };

        class InstanceSpecifier
        {
            public:

            private:
        };


    }
}

#endif
