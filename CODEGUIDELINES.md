Thanks for considering contributing to the project. Before you get to contributing please read over our code guidelines to avoid having us redo your work later. 

These guidelines are a WIP so feel free to contribute to it if you want. Feel free to clone and change it for your own project. ;)

## Formatting
- Tabs with 4 spaces
- {} on new lines
- If lines length are > 250 then break to another line
- Classes/Enums/Structs are to be prefixed with like the others in the project
- No spacing between () or [] etc for example `if ( cond )` or `[ 0 ]`
- Where there are private/protected variables make a Getter/Setter where it makes sense
- Class specifiers are to be in the order of `public`, `protected`, `private`
- Implementations are to be ordered the same as the signature
- Only comment when necessary. Be descriptive with variable names instead. (NO `GetID()`, change to `GetDescriptiveID()`)
- Inline where it makes sense `FORCEINLINE`
- When checking if a pointer is null actually write nullptr `if (ptr != nullptr)`
- In foreach use `auto` if the type is long such as `TPair`
- Pointers/References are to be left (`Ptr* ptr` or `Ref& ref`)
- When using `const` it's to be used like `const bool` or `const Ptr* const` or `Ptr* const`


### Code example
```cpp
class MyClass
{

public:
    UFUNCTION(...)
    int32 GetUniquePlayerID() const { return UniquePlayerID; }

    UFUNCTION(...)
    const FString& GetOnlinePlayerUserName() const { return OnlinePlayerUserName; }

    UFUNCTION(...)
    void SetOnlinePlayerUserName(const FString& NewName);

protected:
private:
    int32 UniquePlayerID;

    FString OnlinePlayerUserName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bTest;
};

// CPP
void MyClass::SetOnlinePlayerUserName(const FString& NewName)
{
    if (NewName.IsEmpty()) 
    {
        return;
    }

    OnlinePlayerUserName = NewName;
}
```

