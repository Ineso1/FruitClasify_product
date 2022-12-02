import React from 'react';
import {NavLink} from 'react-router-dom';

function Header() {
  return (
    <div>
      <ul>
        <li>
            <NavLink to='/' >home</NavLink>
        </li>
        <li>
            <NavLink to='/about' >about</NavLink>
        </li>
        <li>
            <NavLink to='/aafs' >notFound</NavLink>
        </li>
      </ul>
    </div>
  )
}

export default Header
