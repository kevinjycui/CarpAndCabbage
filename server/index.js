const app = require('express')();
const http = require('http').createServer(app);
const io = require('socket.io')(http);

io.on('connection', (socket) => {
	console.log(`connection by socket ${socket.id}`);

	// The socket's room ID
	let socketRoomId;

	socket.on('joinRoom', (roomId) => {
		console.log(`${socket.id} has joined ${roomId}`);
		socket.join(roomId);
		socketRoomId = roomId;

		// If only one person in the room
		if (io.sockets.adapter.rooms[roomId].length <= 1) {
			// Tell client that they are the first in room, client will display
			// something like a waiting screen
			io.to(socket.id).emit('firstInRoom');
		}
		// If the room is full (already has 2 players in it)
		else if (io.sockets.adapter.rooms[roomId].length === 2) {
			io.to(socket.id).emit('fullRoom');
		}
		// Otherwise, have the player join the room and emit the event
		else {
			socket.broadcast.to(roomId).emit('newPlayer', socket.id);
		}
	});

	socket.on('movePlayer', (payload) => {
		console.log(`${socket.id} sends out update move player: ${payload}`)
		socket.broadcast.to(socketRoomId).emit('movePlayer', {
			x: payload.x,
			y: payload.y,
			player: socket.id
		});
	});

	// Cleaning up when a socket disconencts
	socket.on('disconnect', () => {
		socket.removeAllListeners();
	})
})

http.listen(3000, () => {
	console.log('Listening on localhost:3000')
})
