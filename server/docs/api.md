# API


*v1.0.0*


------

1. [Endpoints List](#endpoints-list)
2. [Endpoints Description](#endpoints-description)

------

## Endpoints List
|VERB|URL|
|-----|-----|
|GET| /game/bombs.json
|POST| /game/bombs.json
|DELETE| /game/bombs.json?x=1&y=3
|POST| /game/new.json

------

## Endpoints Description

### `GET` `/game/bombs.json`
#### Description
Retrives all the configured locations coordinates
#### Body parameters format
```json
{}
```
#### Response format (JSON)
```json
[
    {
        "x": 2,
        "y": 3
    },
    {
        "x": 1,
        "y": 3
    }
]
```

### `POST` `/game/bombs.json`
#### Description
Creates a new coordinates configuration in the matrix
#### Body parameters format
```json
{
    "x": 1,
    "y": 3
}
```
#### Response format (JSON)
```json
"Bomb created successfully"
```

### `DELETE` `/game/bombs.json?x=1&y=1` (Query parameters)
#### Description
Given the coordinates, removes the respective configuration
#### Body parameters format
```json
{}
```
#### Response format (JSON)
```json
"Bomb deleted successfully"
```

### `POST` `/game/reset.json`
#### Description
Deletes all the configured coordinates, so no more bombs in the game
#### Body parameters format
```json
{}
```
#### Response format (JSON)
```json
"Game reset"
```