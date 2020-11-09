Thanks for considering contributing to the project. Before you get to contributing please read over our code guidelines to avoid having us redo your work later. 

These guidelines are a WIP so feel free to contribute to it if you want. I'm aware that these guidelines "go against" the UE4 guidelines, but this is the way I like my code. Feel free to clone and change it for your own project. ;)

## Formatting
- Tabs with 4 spaces
- {} on new lines
- If lines length are > 220 then break to another line
- Classes are to be prefixed with like the other classes in the project
- No spacing between () or [] etc for example `if ( cond )` or `[ 0 ]`
- Where there are private/protected variables make a Getter/Setter where it makes sense
- Class specifiers are to be in the order of `public`, `protected`, `private`
- Implementations are to be ordered the same as the signature
- Use m_ as a prefix for variables in the class see below - code uses the [Hungarian Notation](https://en.wikipedia.org/wiki/Hungarian_notation#Examples) format for code. (just read the codebase and you'll see)
- When exposing variables to Blueprint make sure to add the meta tag with DisplayName
- Only comment when necessary. Be descriptive with variable names instead. (NO `GetID()`, change to `GetDescriptiveID()`)
- Inline where it makes sense `FORCEINLINE`
- When checking if a pointer is null actually write nullptr `if (ptr != nullptr)`
- In foreach use `auto` if the type is long such as `TPair`


### Code example
```cpp
class MyClass
{

public:
    UFUNCTION(...)
    int32 GetUniquePlayerID() const { return m_UniquePlayerID; }

    UFUNCTION(...)
    FString GetOnlinePlayerUserName() const { return m_OnlinePlayerUserName; }

    UFUNCTION(...)
    void SetOnlinePlayerUserName(const FString& NewName);

protected:
private:
    int32 m_UniquePlayerID;

    FString m_OnlinePlayerUserName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (DisplayName = "bTest"))
    bool m_bTest;
};

// CPP
void MyClass::SetOnlinePlayerUserName(const FString& NewName)
{
    if (NewName.IsEmpty()) 
    {
        return;
    }

    m_OnlinePlayerUserName = NewName;
}
```

