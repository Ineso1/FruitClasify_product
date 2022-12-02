import React from 'react';
import { BrowserRouter, Route, Routes } from 'react-router-dom';

import HomePage from "./views/HomePage";
import AboutPage from "./views/AboutPage";
import NotFoundPage from "./views/NotFoundPage";

import Header from './components/Header';

function App() {
  return (
    <BrowserRouter>
      {/*<Header/>*/}
      <Routes>
        <Route path='/' element = {<HomePage/>} />
        <Route path='/about' element = {<AboutPage/>} />
        <Route path='*' element = {<NotFoundPage/>} />
      </Routes>
    </BrowserRouter>
  )
}

export default App