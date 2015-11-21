# Goobly:
## Experimental approach to stream processing.

Let's start with an example from spark.

```scala
    val textFile = spark.textFile("hdfs://...")
    val counts = textFile.flatMap(line => line.split(" "))
                     .map(word => (word, 1))
                     .reduceByKey(_ + _)
    counts.saveAsTextFile("hdfs://...")
```

This example is easy to break into a few mental boxes/functions.

```
[source] -> [sentence splitter] -> [emitting word] -> [aggregating results] -> [save]
```

Let's delete the glue and leave only the procesing functions:

```
 [sentence splitter] -> [emitting word] -> [aggregating results]
```

Now, let's break these functions into 2 camps. Those that need state,
and those that don't

Stateless:

```scala

  // stateless funcs
  ...
  .flatMap(line => line.split(" ")) // sentence splitter
  .map(word => (word, 1))           // emitting word

```

Stateful:

```scala
  .reduceByKey(_ + _)

```

## Yay, introducing googly's intended case:

Sorry for the verbose intro, but typing this into english took longer
and was more confusing (#ESLStudent). Feedback is welcomed!

Stateless aggregations are easy to scale. You literally just spin up
more boxes and as long as your routing layer can handle redistribution of work
via consistent hashing (e.g.: 256 vnodes like cassandra or 3 buckets like riak),
stateless computations are the dream for framework authors.

The pseudo code is relatively simple:

```python

   for all downstream subscribers: do
     tell parents to update routing layer # (scale up/down)
     spin up tasks
   done

```

However, statefull operators are much more difficult to scale

```python

   for all downstream subscribers: do
     pause work
     send water mark
     checkpoint state
     tell parents you're done suspending work
     scale up/down
   done

```

Goobly is the component that would make checkpointing state trivial.

Here is what it would look like in practice:

![Intended Architecture](gooblyV1.png)

# BUT WHYYYYYY, for the love of god!
### How do I love 'state', Let me count the ways:


1. Depending on a datastore requires global availability of that datastore
2. It requires you to modify your code, semantics of the biz logic to match
the guarantees of the datastore.
3. When the statsotre is down, the streaming engine has no clue about it and
your supervision strategies no longer make sense.
4. In a world where datacenters are abstracted out as numbers (CPU, Mem, Disk...)
It makes absolutely no sense not to take advantage of it.
5. Recovery of one operator's state is MUCH cheaper than the recovery of a
database node. Consider that you only have to recover the word counts.
6. All stream processing engine have a temporal aspect to them. That is,
after processing data and storing state for the last month, it makes little
sense to keep it on disk, because hopefully you have a materialized view
to represent the final results.
7. Timers and timing operations should be persistent!!!! Reasoning about time
on a strem processing engine is hard AF. Are you going to process them
on event time or system time.


# Let's break it down.
<code>
<img src="http://45.media.tumblr.com/545c97a51ac779ac5607f3cd5e0a3913/tumblr_msesclKS8Z1rdutw3o1_400.gif" height="270" width="480">
</code>

### TODO(agallego):
1. bring all the refs to samza, apex, millwheel, flink, etc
2. Finish the breakdown
