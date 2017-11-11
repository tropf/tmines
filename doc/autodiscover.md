# Autodiscover
The term `Autodiscover` describes a certain feature:

Suppose the folloying Layout:

```
* * * *
* * * 2
* * 1 1
* * 2  
```

One mine is obvious, so you place a flag:

```
* * * *
* * ? 2
* *[1]1
* * 2  
```

When you now click on the field marked with the `[1]`, the program will realise:

- This field is sorrounded by one mine.
- There is **exactly** one mine flagged in the sorroundings.
- Therefore all the other sorrrounding fields are empty.

Then all these sorrounding fields are opened automatically:

```
* * * *
* 2 ? 2
* 1 1 1
* 1 2  
```

This **automated opening** of sorrounding fields is called autodiscover.

> Note that if two mines were flagged (even though only one mine is in the sorrounding 8 fields), autodiscover wouldn't kick in.

Autodiscover itself is implemented in the controller class.

## Autodiscover Only
On startup a `Autodiscover Only` can be activated. When this feature is activated, clicking on a closed field `*` will not display anything.
The **only way** to open fields is via autodiscover.

> The first field is an exception. The first field can be opened directly, but none after that.

It regularly happens that the first field uncovered display only a number, like this:

```
* * *
* 1 *
* * *
```

In which using autodiscover is basically suicide, you can just restart. (Potentially an algorithm can help here later.)
