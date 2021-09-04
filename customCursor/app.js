const cursor = document.querySelector('.cursor');

window.addEventListener('mousemove', (e) => {
    cursor.style.left = e.pageX + 'px'; // in css, value needs Unit of Length;
    cursor.style.top = e.pageY + 'px';  // so we can add string 'px' behind;
    cursor.setAttribute('data-fromTop', (cursor.offsetTop - scrollY));
    console.log(e)
});

window.addEventListener('scroll', () => {
    const fromTop = parseInt(cursor.getAttribute('data-fromTop'))
    cursor.style.top = scrollY + fromTop + 'px';
});

window.addEventListener('click', () => {
    if(cursor.classList.contains('click')){
        cursor.classList.remove('click');
        //Triggering a Dom Reflow;
        void cursor.offsetWidth;
        cursor.classList.add('click');
    }else{
        cursor.classList.add('click');
    }
});



