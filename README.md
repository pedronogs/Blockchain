# Blockchain electoral system with a peer-to-peer architecture

This was a **fourth semester** college project inspired by the topic of Blockchain.

## Inspiration

2018 was the year of elections in Brazil, so we decided to research a lot about technology regarding to this topic. We found out that it is used a highly secure eletronic device for recording votes, but it is a very expensive and slow system, yet it isn't even fault proof. Since our topic was Blockchain, we decided to make a simple implementation of Blockchain technology and compare to our actual voting system.

## Description

> As a matter of fact, I don't really remember many details from when I first made this project but I decided to update the README.md since we had the best grade in that course with this.

As much as I can recall, this implementation followed the architecture of a peer-to-peer network. The real implementation was:
- Each voting machine was a client (where the user inputs his vote) and a server (blockchain synchronization with web sockets).
- Every server was either listening to a client input or another server synchronization call.

Each server stored all the blockchain, so everything was synchronous. The blockchain structure followed the initialization of a genesis block, on each vote the last block hash would be recalculated, compared and used as part of the new block hash (as a matter of visualization, all chain would be verified on each vote to show how an attack would be found). The votes would be public but all personal information would be anonymous (the machine would verify if the person has already voted or not in a list of shuffled records). 

And that is what I remember, good luck trying to run the code !
