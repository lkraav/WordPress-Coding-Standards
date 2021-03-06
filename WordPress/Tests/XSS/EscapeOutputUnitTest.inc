<?php
// Using auto-escaped functions in Loop
while ( have_posts() ) {
	the_tags(); // OK
	the_category(); // OK

	// OK
	?>
	<a href="<?php the_permalink(); ?>" alt="<?php the_title_attribute(); ?>"><?php the_title(); ?></a>
	<?php

	the_content(); // OK
	the_date(); // OK
}
?>

<h2><?php echo $title; // Bad ?></h2>
<h2><?php echo esc_html( $title ); // OK ?></h2>
<h2><?php echo apply_filters( 'the_title', $title ); // Bad, no sanitizing function ?></h2>

<?php
// issue:#53
function custom_column_display( $column, $post_id )
{
    global $post;
    switch ( $column ) {
        case 'some_number' : 
            echo (int) $test;
            echo (int) get_post_meta( $post_id, SOME_NUMBER, true );
        break;
    }
}


$foo = 'abc';
echo $foo; // Bad, should have escaping function
echo 'Some Raw String';  // Good

echo '' . $bad; // Bad, should not validate
echo "this is $bad"; // Bad
echo esc_html( $good . $better ) . $foo; // Bad, should escape all concatenated elements
echo esc_html( $food . 'include'  ); // Good, eveything inside the escaping/sanitizing function should pass
echo esc_html( strtoupper( $ok ) ) . $foo; // Bad, again
echo esc_html( strtoupper( $ok ) ) . ' ' . esc_html( strtolower( $ok ) ); // OK

_e( $some_nasty_var ); // Bad

echo filter_var( $bar, FILTER_VALIDATE_EMAIL );
echo filter_input( INPUT_GET, $bar, FILTER_SANITIZE_SPECIAL_CHARS );

echo '<input type="checkbox" name="' . esc_attr( 'field[' . $id . ']' ) . '" value="on" ' . checked( $current, 'on', false ) . '> ';

echo ent2ncr( $text );

echo number_format ( 1024 );

echo ent2ncr( esc_html( $_data ) );

echo $foo ? $foo : 'no foo'; // Bad
echo empty( $foo ) ? 'no foo' : $foo; // Bad
echo $foo ? esc_html( $foo ) : 'no foo'; // OK

echo 4; // OK

exit( $foo ); // Bad
exit( esc_html( $foo ) ); // OK

die( $foo ); // Bad
die( esc_html( $foo ) ); // OK

printf( 'Hello %s', $foo ); // Bad
printf( 'Hello %s', esc_html( $foo ) ); // OK
printf( 'Hello %s! Hi %s!', esc_html( $foo ), $bar ); // Bad

vprintf( 'Hello %s', array( $foo ) ); // Bad
vprintf( 'Hello %s', array( esc_html( $foo ) ) ); // OK

// The below checks that functions which are marked as needed further sanitization
// don't spill over into later arguments when nested in a function call. There was
// a bug which would cause line 84 to be marked as needing sanitization because _x()
// is marked as needing sanitization.
do_something(
	_x( 'Some string', 'context', 'domain' )
	, array( $foo ) // OK
);

// There was a bug where an empty exit followed by other code would give an error.
if ( ! defined( 'ABSPATH' ) ) {
	exit; // OK
} else {
	other();
}

printf(
	/* translators: this comment is just for you. */
	esc_html__( 'Hello %s.', 'domain' )
	, 'world'
	// There were other long arguments down here "in real life", which is why this was multi-line.
);

wp_die( $message ); // Bad
wp_die( esc_html( $message ) ); // OK
wp_die( esc_html( $message ), $title ); // Bad
wp_die( esc_html( $message ), '', array( 'back_link' => $link_text ) ); // Bad
wp_die( esc_html( $message ), '', array( 'back_link' => esc_html( $link_text ) ) ); // OK
wp_die( esc_html( $message ), '', array( 'response' => 200 ) ); // OK

echo '<h2>', esc_html( $foo ), '</h2>'; // OK
echo 'a', 'b'; // OK
echo 'Hello, ', $foo; // Bad
echo esc_html( $foo ), $bar; // Bad
echo (int) $foo, $bar; // Bad
echo (int) get_post_meta( $post_id, SOME_NUMBER, true ), do_something( $else ); // Bad

wp_die( -1 ); // OK
