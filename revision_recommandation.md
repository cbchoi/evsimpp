# Parallel Execution Recommendations

## Current Architecture Observations
- `CSystemExecutor::schedule` processes the next event strictly on a single thread, pulling one executor item at a time from `m_schedule_list`.
- Global containers (`m_schedule_list`, `m_wait_object_list`, `m_model_executor_map`) are shared across all models, forcing serialization on insert/erase.
- Message routing (`route_message`) and `MessageDeliverer` rely on `std::multiset`, assuming single-threaded access.

## Recommended Parallelisation Steps
1. **Batch Same-Time Events**
   - Extract all executor items whose `next_event_t` equals the current global time and process them as a batch.
   - Dispatch the batch to a worker pool so `output_function` and `internal_transition` run in parallel when models are independent.

2. **Thread-Safe Schedulers**
   - Replace the `std::multiset` scheduler with a priority queue protected by fine-grained locks or a concurrent heap.
   - Cache iterators per executor to avoid repeated erase/insert cycles.

3. **Asynchronous Message Delivery**
   - Promote `MessageDeliverer` to a concurrent priority queue so routing can occur off the main thread.
   - Deliver messages through lock-free channels to destination executors; merge back into the main scheduler once transitions complete.

4. **Background Entity Creation**
   - Move `m_wait_object_list` draining into a background task that prepares executors ahead of the simulation clock.
   - Maintain lock-free handoff structures so the main loop simply swaps in ready executors.

5. **Execution Modes and Safety**
   - Introduce explicit configuration for parallel execution modes (disabled/batched/message-routing).
   - Gate parallel paths behind config flags until full thread-safety and determinism are verified by targeted tests.

## Validation Strategy
- Extend the GoogleTest suite with benchmarks comparing serial vs. candidate parallel modes.
- Track timing metrics via `RecordProperty` for reproducible regression analysis.
