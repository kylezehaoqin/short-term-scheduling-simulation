
class Rand( object ):

	def _init_( self, seed ):
		self.seed = seed

	def srand48( self, seed ):
		self.seed = ( seed << 16 ) + 0x330e

	def next( self ):
		self.seed = ( 25214903917 * self.seed + 11 ) & ( 2**48 - 1 )

	def drand48( self ):
		return self.next() / 2 ** 48

if __name__ == "__main__":


	n = 16
	# Define seed
	seed = 2

	rand = Rand( seed )



	for i in range(n):
		pid = str( chr( i + 65 ) )
		
		



	print()
